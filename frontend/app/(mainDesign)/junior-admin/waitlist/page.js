"use client";

import { useState, useEffect } from "react";
import { useRouter } from 'next/navigation';
import Link from "next/link";

export default function JuniorAdminWaitlistPage() {
    const [loading, setLoading] = useState(true);
    const [error, setError] = useState("");
    const router = useRouter();

    // Данные администратора
    const [adminId, setAdminId] = useState(null);
    const [hospitalId, setHospitalId] = useState(null);
    const [hospitalName, setHospitalName] = useState("");

    // Данные для листа ожидания
    const [doctors, setDoctors] = useState([]);
    const [selectedDoctor, setSelectedDoctor] = useState(null);
    const [waitlist, setWaitlist] = useState([]);
    const [loadingWaitlist, setLoadingWaitlist] = useState(false);

    useEffect(() => {
        // Проверка аутентификации
        const isLoggedIn = localStorage.getItem('isLoggedIn');
        const userData = JSON.parse(localStorage.getItem('medSchedulerUser') || '{}');

        if (!isLoggedIn || !userData.userId || userData.userType !== 'junior') {
            router.push('/login');
            return;
        }

        setAdminId(userData.userId);

        // Получаем информацию о больнице администратора
        fetchAdminHospital(userData.userId);
    }, [router]);

    const fetchAdminHospital = async (adminId) => {
        try {
            const response = await fetch(`https://api.medscheduler.ru/get_admin_hospital?admin_id=${adminId}`);
            const data = await response.json();

            if (response.ok && data.success) {
                setHospitalId(data.hospital.hospital_id);
                setHospitalName(data.hospital.full_name);

                // После получения данных о больнице, загружаем врачей этой больницы
                fetchDoctors(data.hospital.hospital_id);
            } else {
                setError(data.error || "Не удалось получить информацию о больнице");
                setLoading(false);
            }
        } catch (err) {
            console.error("Error fetching admin hospital:", err);
            setError("Не удалось подключиться к серверу");
            setLoading(false);
        }
    };

    const fetchDoctors = async (hospitalId) => {
        try {
            const response = await fetch(`https://api.medscheduler.ru/get_hospital_doctors?hospital_id=${hospitalId}`);
            const data = await response.json();

            if (response.ok && data.success) {
                setDoctors(data.doctors);
            } else {
                setError(data.error || "Не удалось загрузить список врачей");
            }
        } catch (err) {
            console.error("Error fetching doctors:", err);
            setError("Не удалось подключиться к серверу");
        } finally {
            setLoading(false);
        }
    };

    const fetchWaitlist = async (doctorId) => {
        if (!doctorId || !adminId) return;

        setLoadingWaitlist(true);
        try {
            const response = await fetch(`https://api.medscheduler.ru/get_waitlist?doctor_id=${doctorId}&junior_admin_id=${adminId}`);
            const data = await response.json();

            if (response.ok && data.success) {
                setWaitlist(data.waitlist);
            } else {
                setError(data.error || "Ошибка при получении листа ожидания");
            }
        } catch (err) {
            console.error("Error fetching waitlist:", err);
            setError("Не удалось подключиться к серверу");
        } finally {
            setLoadingWaitlist(false);
        }
    };

    const handleDoctorChange = (e) => {
        const doctorId = e.target.value;
        if (doctorId) {
            const doctor = doctors.find(d => d.doctor_id === parseInt(doctorId));
            setSelectedDoctor(doctor);
            fetchWaitlist(doctorId);
        } else {
            setSelectedDoctor(null);
            setWaitlist([]);
        }
    };

    // Форматирование даты
    const formatDate = (dateStr) => {
        const date = new Date(dateStr);
        return date.toLocaleDateString('ru-RU', {
            day: '2-digit',
            month: '2-digit',
            year: 'numeric'
        });
    };

    if (loading) {
        return (
            <div className="min-h-[calc(80vh-theme(space.36))] flex items-center justify-center">
                <div className="text-xl text-gray-600">Загрузка данных...</div>
            </div>
        );
    }

    return (
        <div className="max-w-6xl mx-auto py-8 px-4">
            <h1 className="text-3xl font-bold text-main2 mb-8">Лист ожидания</h1>

            {error && (
                <div className="bg-red-100 border border-red-400 text-red-700 px-4 py-3 rounded mb-4">
                    {error}
                </div>
            )}

            <div className="bg-white shadow-md rounded-lg p-6 mb-6">
                <div className="flex flex-col md:flex-row justify-between md:items-center mb-6">
                    <div>
                        <h2 className="text-xl font-semibold text-main2">Ваша больница</h2>
                        <p className="text-gray-600 mt-1">{hospitalName || "Больница не привязана"}</p>
                    </div>
                </div>

                <div className="mb-6">
                    <label htmlFor="doctorSelect" className="block text-sm font-medium text-gray-700 mb-2">
                        Выберите врача для просмотра листа ожидания
                    </label>
                    <select
                        id="doctorSelect"
                        onChange={handleDoctorChange}
                        className="w-full px-3 py-2 border border-gray-300 rounded-md focus:outline-none focus:ring-1 focus:ring-main"
                        defaultValue=""
                    >
                        <option value="" disabled>Выберите врача</option>
                        {doctors.map((doctor) => (
                            <option key={doctor.doctor_id} value={doctor.doctor_id}>
                                {doctor.last_name} {doctor.first_name} {doctor.patronymic} - {doctor.specialty}
                            </option>
                        ))}
                    </select>
                </div>

                {selectedDoctor && (
                    <div>
                        <h3 className="text-lg font-medium text-gray-800 mb-4">
                            Лист ожидания к {selectedDoctor.last_name} {selectedDoctor.first_name} {selectedDoctor.patronymic}
                        </h3>

                        {loadingWaitlist ? (
                            <div className="text-center py-6">
                                <p className="text-gray-600">Загрузка данных...</p>
                            </div>
                        ) : waitlist.length === 0 ? (
                            <div className="text-center py-6 bg-gray-50 rounded-lg">
                                <p className="text-gray-600">Лист ожидания пуст</p>
                            </div>
                        ) : (
                            <div className="overflow-x-auto">
                                <table className="min-w-full divide-y divide-gray-200">
                                    <thead className="bg-gray-50">
                                        <tr>
                                            <th scope="col" className="px-6 py-3 text-left text-xs font-medium text-gray-500 uppercase tracking-wider">
                                                Позиция
                                            </th>
                                            <th scope="col" className="px-6 py-3 text-left text-xs font-medium text-gray-500 uppercase tracking-wider">
                                                Пациент
                                            </th>
                                            <th scope="col" className="px-6 py-3 text-left text-xs font-medium text-gray-500 uppercase tracking-wider">
                                                Телефон
                                            </th>
                                            <th scope="col" className="px-6 py-3 text-left text-xs font-medium text-gray-500 uppercase tracking-wider">
                                                Дата добавления
                                            </th>
                                        </tr>
                                    </thead>
                                    <tbody className="bg-white divide-y divide-gray-200">
                                        {waitlist.map((item, index) => (
                                            <tr key={item.id}>
                                                <td className="px-6 py-4 whitespace-nowrap">
                                                    <div className="text-sm font-medium text-gray-900">
                                                        {index + 1}
                                                    </div>
                                                </td>
                                                <td className="px-6 py-4 whitespace-nowrap">
                                                    <div className="text-sm text-gray-900">
                                                        {item.patient_name || `${item.last_name || ""} ${item.first_name || ""} ${item.patronymic || ""}`}
                                                    </div>
                                                </td>
                                                <td className="px-6 py-4 whitespace-nowrap">
                                                    <div className="text-sm text-gray-500">
                                                        {item.phone}
                                                    </div>
                                                </td>
                                                <td className="px-6 py-4 whitespace-nowrap">
                                                    <div className="text-sm text-gray-500">
                                                        {formatDate(item.date_added || item.added_date)}
                                                    </div>
                                                </td>
                                            </tr>
                                        ))}
                                    </tbody>
                                </table>
                            </div>
                        )}
                    </div>
                )}
            </div>

            <div className="bg-white shadow-md rounded-lg p-6 mb-6">
                <h2 className="text-xl font-semibold text-main2 mb-4">О листе ожидания</h2>
                <div className="prose prose-sm max-w-none text-gray-600">
                    <p>
                        Лист ожидания позволяет пациентам записаться в очередь к врачу, если нет свободных слотов для записи.
                        При отмене записи пациентом система автоматически уведомляет первого пациента из листа ожидания о возможности записи.
                    </p>
                    <p>
                        Приоритет в листе ожидания определяется порядком добавления пациентов (первым пришел — первым обслужен).
                    </p>
                </div>
            </div>

            <div className="flex justify-between">
                <Link href="/junior-admin/schedule" className="text-main hover:text-main2 transition-colors">
                    ← Управление расписанием
                </Link>
                <Link href="/junior-admin/doctor-stats" className="text-main hover:text-main2 transition-colors">
                    Статистика по врачам →
                </Link>
            </div>
        </div>
    );
}
