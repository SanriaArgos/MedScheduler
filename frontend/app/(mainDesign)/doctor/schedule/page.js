"use client";

import { useState, useEffect } from "react";
import { useRouter } from 'next/navigation';
import Link from "next/link";

export default function DoctorSchedulePage() {
    const [loading, setLoading] = useState(true);
    const [schedule, setSchedule] = useState({});
    const [error, setError] = useState("");
    const router = useRouter();

    const [selectedDate, setSelectedDate] = useState(null);
    const [allDates, setAllDates] = useState([]);
    const [userId, setUserId] = useState(null);

    useEffect(() => {
        // Проверка аутентификации
        const isLoggedIn = localStorage.getItem('isLoggedIn');
        const userData = JSON.parse(localStorage.getItem('medSchedulerUser') || '{}');

        if (!isLoggedIn || !userData.userId || userData.userType !== 'doctor') {
            router.push('/login');
            return;
        }

        setUserId(userData.userId);
        fetchSchedule(userData.userId);
    }, [router]);

    const fetchSchedule = async (doctorId) => {
        try {
            const response = await fetch(`https://api.medscheduler.ru/get_doctor_schedule?doctor_id=${doctorId}`);
            const data = await response.json();

            if (response.ok && data.success) {
                // Группируем слоты по датам
                const groupedByDate = data.schedule.reduce((acc, slot) => {
                    const date = slot.appointment_date;
                    if (!acc[date]) {
                        acc[date] = [];
                    }
                    acc[date].push(slot);
                    return acc;
                }, {});

                // Сортировка дат
                const sortedDates = Object.keys(groupedByDate).sort();

                setSchedule(groupedByDate);
                setAllDates(sortedDates);

                // Выбираем первую дату по умолчанию, если есть записи
                if (sortedDates.length > 0) {
                    setSelectedDate(sortedDates[0]);
                }
            } else {
                setError(data.error || "Ошибка при получении расписания");
            }
        } catch (err) {
            console.error("Error fetching schedule:", err);
            setError("Не удалось подключиться к серверу");
        } finally {
            setLoading(false);
        }
    };

    // Форматирование даты
    const formatDate = (dateStr) => {
        const date = new Date(dateStr);
        return date.toLocaleDateString('ru-RU', {
            day: '2-digit',
            month: '2-digit',
            year: 'numeric',
            weekday: 'long'
        });
    };

    // Форматирование времени
    const formatTime = (timeStr) => {
        return timeStr.substring(0, 5); // Берем только HH:MM из HH:MM:SS
    };

    // Проверка, прошла ли уже дата/время
    const isPast = (dateStr, timeStr) => {
        const dateTime = new Date(`${dateStr}T${timeStr}`);
        return dateTime < new Date();
    };

    if (loading) {
        return (
            <div className="min-h-[calc(80vh-theme(space.36))] flex items-center justify-center">
                <div className="text-xl text-gray-600">Загрузка расписания...</div>
            </div>
        );
    }

    return (
        <div className="max-w-5xl mx-auto py-8 px-4">
            <h1 className="text-3xl font-bold text-main2 mb-8">Мое расписание</h1>

            {error && (
                <div className="bg-red-100 border border-red-400 text-red-700 px-4 py-3 rounded mb-4">
                    {error}
                </div>
            )}

            {allDates.length === 0 ? (
                <div className="bg-white shadow-md rounded-lg p-6 text-center">
                    <p className="text-lg text-gray-600 mb-4">У вас нет запланированных приемов</p>
                    <p className="text-gray-500">Младший администратор клиники должен добавить слоты для записи в ваше расписание</p>
                </div>
            ) : (
                <div className="grid grid-cols-1 md:grid-cols-4 gap-6">
                    {/* Выбор даты - боковая панель */}
                    <div className="md:col-span-1">
                        <div className="bg-white shadow-md rounded-lg p-4">
                            <h2 className="text-lg font-semibold text-main2 mb-4">Выберите дату</h2>
                            <div className="space-y-2">
                                {allDates.map((date) => (
                                    <button
                                        key={date}
                                        onClick={() => setSelectedDate(date)}
                                        className={`w-full text-left px-3 py-2 rounded-md transition-colors ${
                                            selectedDate === date
                                            ? "bg-main text-white"
                                            : isPast(date, "23:59:59") 
                                              ? "bg-gray-100 text-gray-500"
                                              : "bg-gray-100 text-gray-700 hover:bg-gray-200"
                                        }`}
                                    >
                                        {formatDate(date)}
                                        <span className="block text-xs mt-1">
                                            {schedule[date].length} {schedule[date].length === 1 ? "запись" :
                                            schedule[date].length > 1 && schedule[date].length < 5 ? "записи" : "записей"}
                                        </span>
                                    </button>
                                ))}
                            </div>
                        </div>
                    </div>

                    {/* Расписание на выбранную дату */}
                    <div className="md:col-span-3">
                        {selectedDate ? (
                            <div className="bg-white shadow-md rounded-lg overflow-hidden">
                                <div className="px-6 py-4 border-b border-gray-200">
                                    <h2 className="text-xl font-semibold text-main2">
                                        Расписание на {formatDate(selectedDate)}
                                    </h2>
                                </div>

                                <table className="min-w-full divide-y divide-gray-200">
                                    <thead className="bg-gray-50">
                                        <tr>
                                            <th scope="col" className="px-6 py-3 text-left text-xs font-medium text-gray-500 uppercase tracking-wider">
                                                Время
                                            </th>
                                            <th scope="col" className="px-6 py-3 text-left text-xs font-medium text-gray-500 uppercase tracking-wider">
                                                Статус
                                            </th>
                                            <th scope="col" className="px-6 py-3 text-left text-xs font-medium text-gray-500 uppercase tracking-wider">
                                                Клиника
                                            </th>
                                            <th scope="col" className="px-6 py-3 text-left text-xs font-medium text-gray-500 uppercase tracking-wider">
                                                Кабинет
                                            </th>
                                        </tr>
                                    </thead>
                                    <tbody className="bg-white divide-y divide-gray-200">
                                        {schedule[selectedDate].sort((a, b) => a.appointment_time.localeCompare(b.appointment_time)).map((slot) => {
                                            const isSlotPast = isPast(slot.appointment_date, slot.appointment_time);

                                            return (
                                                <tr key={`${slot.appointment_date}-${slot.appointment_time}-${slot.cabinet}`} className={isSlotPast ? "bg-gray-50" : ""}>
                                                    <td className="px-6 py-4 whitespace-nowrap">
                                                        <div className="text-sm font-medium text-gray-900">
                                                            {formatTime(slot.appointment_time)}
                                                        </div>
                                                    </td>
                                                    <td className="px-6 py-4 whitespace-nowrap">
                                                        {slot.patient_name ? (
                                                            <div className="flex items-center">
                                                                <div className="flex-shrink-0 h-10 w-10 bg-main2 rounded-full flex items-center justify-center text-white">
                                                                    {slot.patient_name.charAt(0)}
                                                                </div>
                                                                <div className="ml-4">
                                                                    <div className="text-sm font-medium text-gray-900">
                                                                        {slot.patient_name}
                                                                    </div>
                                                                    <div className="text-sm text-gray-500">
                                                                        Запись подтверждена
                                                                    </div>
                                                                </div>
                                                            </div>
                                                        ) : isSlotPast ? (
                                                            <span className="px-2 inline-flex text-xs leading-5 font-semibold rounded-full bg-gray-100 text-gray-800">
                                                                Прошедшее время
                                                            </span>
                                                        ) : (
                                                            <span className="px-2 inline-flex text-xs leading-5 font-semibold rounded-full bg-green-100 text-green-800">
                                                                Свободно
                                                            </span>
                                                        )}
                                                    </td>
                                                    <td className="px-6 py-4 whitespace-nowrap">
                                                        <div className="text-sm text-gray-900">{slot.full_name}</div>
                                                    </td>
                                                    <td className="px-6 py-4 whitespace-nowrap text-sm text-gray-500">
                                                        {slot.cabinet}
                                                    </td>
                                                </tr>
                                            );
                                        })}
                                    </tbody>
                                </table>
                            </div>
                        ) : (
                            <div className="bg-white shadow-md rounded-lg p-6 text-center">
                                <p className="text-gray-600">Выберите дату для просмотра расписания</p>
                            </div>
                        )}
                    </div>
                </div>
            )}

            <div className="flex justify-between mt-8">
                <Link href="/doctor/profile" className="text-main hover:text-main2 transition-colors">
                    ← Мой профиль
                </Link>
            </div>
        </div>
    );
}
