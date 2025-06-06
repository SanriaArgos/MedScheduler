"use client";

import { useState, useEffect } from "react";
import { useRouter } from 'next/navigation';
import Link from "next/link";

export default function DoctorStatsPage() {
    const [loading, setLoading] = useState(true);
    const [error, setError] = useState("");
    const router = useRouter();

    // Данные администратора
    const [adminId, setAdminId] = useState(null);
    const [hospitalId, setHospitalId] = useState(null);
    const [hospitalName, setHospitalName] = useState("");

    // Данные для статистики
    const [doctors, setDoctors] = useState([]);
    const [doctorStats, setDoctorStats] = useState({});
    const [loadingStats, setLoadingStats] = useState(false);
    const [selectedDoctor, setSelectedDoctor] = useState(null);
    const [searchQuery, setSearchQuery] = useState("");
    const [filteredDoctors, setFilteredDoctors] = useState([]);

    // Состояние для выбора периода
    const [period, setPeriod] = useState("month"); // "week", "month", "year"

    useEffect(() => {
        // Проверка аутентификации
        const isLoggedIn = localStorage.getItem('isLoggedIn');
        const userData = JSON.parse(localStorage.getItem('medSchedulerUser') || '{}');

        if (!isLoggedIn || !userData.userId || userData.userType !== 'junior') {
            router.push('/login');
            return;
        }

        setAdminId(userData.userId);

        // Здесь должен быть запрос для получения ID больницы, привязанной к администратору
        // Поскольку у нас нет такого API, мы используем моковые данные
        const mockHospitalData = {
            hospitalId: 1,
            fullName: "Городская поликлиника №1"
        };

        setHospitalId(mockHospitalData.hospitalId);
        setHospitalName(mockHospitalData.fullName);

        // Получаем список врачей больницы
        fetchDoctors();
    }, [router]);

    useEffect(() => {
        // Фильтрация врачей при изменении поискового запроса
        if (searchQuery) {
            const lowercaseQuery = searchQuery.toLowerCase();
            const filtered = doctors.filter(
                doctor =>
                    doctor.last_name.toLowerCase().includes(lowercaseQuery) ||
                    doctor.first_name.toLowerCase().includes(lowercaseQuery) ||
                    (doctor.patronymic && doctor.patronymic.toLowerCase().includes(lowercaseQuery)) ||
                    doctor.specialty.toLowerCase().includes(lowercaseQuery) ||
                    doctor.phone.toLowerCase().includes(lowercaseQuery)
            );
            setFilteredDoctors(filtered);
        } else {
            setFilteredDoctors(doctors);
        }
    }, [searchQuery, doctors]);

    const fetchDoctors = async () => {
        try {
            // В реальном приложении здесь будет запрос к API для получения
            // списка врачей, прикрепленных к больнице администратора

            // Моковые данные для примера
            const mockDoctors = [
                {
                    doctor_id: 1,
                    last_name: "Иванов",
                    first_name: "Алексей",
                    patronymic: "Петрович",
                    specialty: "Кардиолог",
                    phone: "79001234567"
                },
                {
                    doctor_id: 2,
                    last_name: "Петрова",
                    first_name: "Елена",
                    patronymic: "Сергеевна",
                    specialty: "Невролог",
                    phone: "79002345678"
                },
                {
                    doctor_id: 3,
                    last_name: "Сидоров",
                    first_name: "Иван",
                    patronymic: "Алексеевич",
                    specialty: "Терапевт",
                    phone: "79003456789"
                }
            ];

            setDoctors(mockDoctors);
            setFilteredDoctors(mockDoctors);

            // Моковые данные статистики для всех врачей
            const mockStats = {};
            mockDoctors.forEach(doctor => {
                mockStats[doctor.doctor_id] = {
                    patients_count: Math.floor(Math.random() * 100) + 10,
                    average_rating: (Math.random() * 2 + 3).toFixed(1), // От 3.0 до 5.0
                    price: (Math.floor(Math.random() * 20) + 10) * 100 // От 1000 до 3000
                };
            });

            setDoctorStats(mockStats);
        } catch (err) {
            console.error("Error fetching doctors:", err);
            setError("Не удалось загрузить список врачей");
        } finally {
            setLoading(false);
        }
    };

    const fetchDoctorStats = async (doctorPhone) => {
        if (!doctorPhone || !adminId) return;

        setLoadingStats(true);
        try {
            const response = await fetch('https://api.medscheduler.ru/get_doctor_statistics', {
                method: 'POST',
                headers: { 'Content-Type': 'application/json' },
                body: JSON.stringify({
                    junior_admin_id: parseInt(adminId),
                    doctor_phone: doctorPhone
                }),
            });

            const data = await response.json();

            if (response.ok && data.success) {
                // Обновляем статистику для выбранного врача
                setDoctorStats(prevStats => ({
                    ...prevStats,
                    [selectedDoctor.doctor_id]: {
                        patients_count: data.patients_count,
                        average_rating: data.average_rating,
                        price: data.price
                    }
                }));
            } else {
                setError(data.error || "Ошибка при получении статистики врача");
            }
        } catch (err) {
            console.error("Error fetching doctor stats:", err);
            setError("Не удалось подключиться к серверу");
        } finally {
            setLoadingStats(false);
        }
    };

    const handleDoctorSelect = (doctor) => {
        setSelectedDoctor(doctor);
        fetchDoctorStats(doctor.phone);
    };

    const handlePeriodChange = (newPeriod) => {
        setPeriod(newPeriod);
        // В реальном приложении здесь был бы запрос за указанный период
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
            <h1 className="text-3xl font-bold text-main2 mb-8">Статистика по врачам</h1>

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
                    <div className="mt-4 md:mt-0">
                        <div className="flex space-x-2">
                            <button
                                onClick={() => handlePeriodChange("week")}
                                className={`px-3 py-1 rounded-md ${
                                    period === "week"
                                    ? "bg-main text-white"
                                    : "bg-gray-100 text-gray-700 hover:bg-gray-200"
                                }`}
                            >
                                Неделя
                            </button>
                            <button
                                onClick={() => handlePeriodChange("month")}
                                className={`px-3 py-1 rounded-md ${
                                    period === "month"
                                    ? "bg-main text-white"
                                    : "bg-gray-100 text-gray-700 hover:bg-gray-200"
                                }`}
                            >
                                Месяц
                            </button>
                            <button
                                onClick={() => handlePeriodChange("year")}
                                className={`px-3 py-1 rounded-md ${
                                    period === "year"
                                    ? "bg-main text-white"
                                    : "bg-gray-100 text-gray-700 hover:bg-gray-200"
                                }`}
                            >
                                Год
                            </button>
                        </div>
                    </div>
                </div>

                {/* Поиск врачей */}
                <div className="mb-6">
                    <label htmlFor="search" className="block text-sm font-medium text-gray-700 mb-1">
                        Поиск врачей
                    </label>
                    <input
                        type="text"
                        id="search"
                        value={searchQuery}
                        onChange={(e) => setSearchQuery(e.target.value)}
                        placeholder="Поиск по ФИО, специальности или телефону"
                        className="w-full px-3 py-2 border border-gray-300 rounded-md focus:outline-none focus:ring-1 focus:ring-main"
                    />
                </div>

                <div className="grid grid-cols-1 lg:grid-cols-3 gap-6">
                    {/* Список врачей */}
                    <div className="lg:col-span-1 bg-gray-50 rounded-lg p-4">
                        <h3 className="text-lg font-medium text-gray-800 mb-4">Врачи</h3>

                        {filteredDoctors.length === 0 ? (
                            <p className="text-gray-500 text-center py-4">Врачи не найдены</p>
                        ) : (
                            <div className="space-y-2 max-h-[400px] overflow-y-auto">
                                {filteredDoctors.map((doctor) => (
                                    <button
                                        key={doctor.doctor_id}
                                        onClick={() => handleDoctorSelect(doctor)}
                                        className={`w-full text-left px-4 py-3 rounded-md transition-colors ${
                                            selectedDoctor && selectedDoctor.doctor_id === doctor.doctor_id
                                            ? "bg-main text-white"
                                            : "bg-white text-gray-700 hover:bg-gray-100"
                                        }`}
                                    >
                                        <div className="font-medium">{doctor.last_name} {doctor.first_name} {doctor.patronymic}</div>
                                        <div className="text-sm mt-1 ${selectedDoctor && selectedDoctor.doctor_id === doctor.doctor_id ? 'text-white' : 'text-gray-500'}">
                                            {doctor.specialty}
                                        </div>
                                    </button>
                                ))}
                            </div>
                        )}
                    </div>

                    {/* Статистика врача */}
                    <div className="lg:col-span-2">
                        {!selectedDoctor ? (
                            <div className="bg-white border border-gray-200 rounded-lg p-6 text-center">
                                <p className="text-gray-600">Выберите врача для просмотра статистики</p>
                            </div>
                        ) : loadingStats ? (
                            <div className="bg-white border border-gray-200 rounded-lg p-6 text-center">
                                <p className="text-gray-600">Загрузка статистики...</p>
                            </div>
                        ) : (
                            <div className="bg-white border border-gray-200 rounded-lg overflow-hidden">
                                <div className="px-6 py-4 bg-gray-50 border-b border-gray-200">
                                    <h3 className="text-lg font-medium text-gray-800">
                                        {selectedDoctor.last_name} {selectedDoctor.first_name} {selectedDoctor.patronymic}
                                    </h3>
                                    <p className="text-sm text-gray-500 mt-1">{selectedDoctor.specialty}</p>
                                </div>

                                <div className="p-6">
                                    <div className="grid grid-cols-1 md:grid-cols-3 gap-6 mb-6">
                                        <div className="bg-gray-50 rounded-lg p-4 text-center">
                                            <p className="text-sm text-gray-500 mb-1">Количество пациентов</p>
                                            <p className="text-2xl font-bold text-main2">
                                                {doctorStats[selectedDoctor.doctor_id]?.patients_count || 0}
                                            </p>
                                            <p className="text-xs text-gray-500 mt-1">
                                                за {period === "week" ? "неделю" : period === "month" ? "месяц" : "год"}
                                            </p>
                                        </div>
                                        <div className="bg-gray-50 rounded-lg p-4 text-center">
                                            <p className="text-sm text-gray-500 mb-1">Средний рейтинг</p>
                                            <div className="flex items-center justify-center">
                                                <p className="text-2xl font-bold text-main2 mr-1">
                                                    {doctorStats[selectedDoctor.doctor_id]?.average_rating || "—"}
                                                </p>
                                                <svg xmlns="http://www.w3.org/2000/svg" className="h-5 w-5 text-yellow-500" viewBox="0 0 20 20" fill="currentColor">
                                                    <path d="M9.049 2.927c.3-.921 1.603-.921 1.902 0l1.07 3.292a1 1 0 00.95.69h3.462c.969 0 1.371 1.24.588 1.81l-2.8 2.034a1 1 0 00-.364 1.118l1.07 3.292c.3.921-.755 1.688-1.54 1.118l-2.8-2.034a1 1 0 00-1.175 0l-2.8 2.034c-.784.57-1.838-.197-1.539-1.118l1.07-3.292a1 1 0 00-.364-1.118L2.98 8.72c-.783-.57-.38-1.81.588-1.81h3.461a1 1 0 00.951-.69l1.07-3.292z" />
                                                </svg>
                                            </div>
                                        </div>
                                        <div className="bg-gray-50 rounded-lg p-4 text-center">
                                            <p className="text-sm text-gray-500 mb-1">Цена приема</p>
                                            <p className="text-2xl font-bold text-main2">
                                                {doctorStats[selectedDoctor.doctor_id]?.price || 0} ₽
                                            </p>
                                        </div>
                                    </div>

                                    <div className="mb-6">
                                        <h4 className="text-md font-medium text-gray-700 mb-3">
                                            Загруженность по дням недели
                                        </h4>
                                        <div className="bg-gray-50 rounded-lg p-4">
                                            <div className="space-y-3">
                                                {["Понедельник", "Вторник", "Среда", "Четверг", "Пятница"].map(day => {
                                                    const percentage = Math.floor(Math.random() * 90) + 10; // Рандомное значение от 10% до 100%
                                                    return (
                                                        <div key={day}>
                                                            <div className="flex justify-between text-sm mb-1">
                                                                <span className="text-gray-700">{day}</span>
                                                                <span className="text-gray-700">{percentage}%</span>
                                                            </div>
                                                            <div className="w-full bg-gray-200 rounded-full h-2.5">
                                                                <div className="bg-main h-2.5 rounded-full" style={{ width: `${percentage}%` }}></div>
                                                            </div>
                                                        </div>
                                                    );
                                                })}
                                            </div>
                                        </div>
                                    </div>

                                    <div>
                                        <h4 className="text-md font-medium text-gray-700 mb-3">
                                            Статистика отмен
                                        </h4>
                                        <div className="grid grid-cols-1 md:grid-cols-2 gap-4">
                                            <div className="bg-gray-50 rounded-lg p-4 text-center">
                                                <p className="text-sm text-gray-500 mb-1">Отмененные записи</p>
                                                <p className="text-2xl font-bold text-red-500">
                                                    {Math.floor(Math.random() * 5)}
                                                </p>
                                                <p className="text-xs text-gray-500 mt-1">
                                                    за {period === "week" ? "неделю" : period === "month" ? "месяц" : "год"}
                                                </p>
                                            </div>
                                            <div className="bg-gray-50 rounded-lg p-4 text-center">
                                                <p className="text-sm text-gray-500 mb-1">Процент отмен</p>
                                                <p className="text-2xl font-bold text-orange-500">
                                                    {Math.floor(Math.random() * 5) + 1}%
                                                </p>
                                                <p className="text-xs text-gray-500 mt-1">
                                                    от общего числа записей
                                                </p>
                                            </div>
                                        </div>
                                    </div>
                                </div>
                            </div>
                        )}
                    </div>
                </div>
            </div>

            <div className="flex justify-between">
                <Link href="/junior-admin/waitlist" className="text-main hover:text-main2 transition-colors">
                    ← Лист ожидания
                </Link>
                <Link href="/junior-admin/profile" className="text-main hover:text-main2 transition-colors">
                    Профиль →
                </Link>
            </div>
        </div>
    );
}
